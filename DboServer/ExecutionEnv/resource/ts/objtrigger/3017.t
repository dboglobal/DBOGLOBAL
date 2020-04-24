CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3017;
	title = 301702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 6;
			prelnk = "5;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 301718;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 254;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckCustomEvent
			{
				qid = 3017;
			}
			CDboTSCheckAttachObj
			{
				objidx = "2200;2201;2202;2203;2204;2205;2206;2207;";
				widx = 1;
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
				ceid = 3017;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 13;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "13;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 1;
				taid = 1;
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
				time = 5000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 12;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 25000;
			}
		}
	}
}

