CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 122;
	title = 12202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 12;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 401;
				osh_sh = 255;
				widx = 1;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "5;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 433;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "51;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 401;
				osh_sh = 255;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 10000;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "202;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 401;
				osh_sh = 255;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 51;
			prelnk = "12;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 12218;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 12207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "401;";
				widx = 1;
			}
			CDboTSCheckObjItem
			{
				tid = 122;
			}
		}
	}
}

