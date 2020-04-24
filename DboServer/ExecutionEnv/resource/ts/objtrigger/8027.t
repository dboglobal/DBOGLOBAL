CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8027;
	title = 802702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 10;
			prelnk = "0;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 254;
			prelnk = "14;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 15;
			prelnk = "13;";

			CDboTSActObjWPS
			{
				sid = 26014;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 17;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 14;
			prelnk = "12;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 17;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "15;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 802707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckAttachObj
			{
				objidx = "17;";
				widx = 310000;
			}
		}
	}
}

