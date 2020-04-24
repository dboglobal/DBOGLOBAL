CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2967;
	title = 296702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 1919;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 0;
				oidx = 1920;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 15000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 1920;
				osh_sh = 255;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 1919;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "0;";

			CDboTSActCustomEvt
			{
				ceid = 2967;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 296707;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckCustomEvent
			{
				qid = 2967;
			}
			CDboTSCheckAttachObj
			{
				objidx = "1919;";
				widx = 1;
			}
		}
	}
}

