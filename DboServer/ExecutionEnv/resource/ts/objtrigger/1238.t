CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1238;
	title = 123802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

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
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 10000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActPCConv
			{
				taid = 2;
				tblidx = 123818;
			}
			CDboTSActTWaitTS
			{
				taid = 3;
				time = 1000;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 800;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 1;
			prelnk = "12;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 12;
			prelnk = "11;";
			nolnk = 254;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 123807;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckObjItem
			{
				tid = 1238;
			}
			CDboTSClickObject
			{
				objidx = "263;264;265;266;267;268;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 2000;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;12;";
			type = 1;
		}
	}
}

