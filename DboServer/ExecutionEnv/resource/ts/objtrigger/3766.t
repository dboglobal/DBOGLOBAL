CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3766;
	title = 376602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 51;
			prelnk = "11;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 376618;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 3766;
			}
			CDboTSClickObject
			{
				objidx = "28;29;30;31;32;33;34;35;36;37;80;81;82;83;84;85;86;87;88;89;";
				widx = 15;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 11;
			prelnk = "2;";

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
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "202;";

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
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 45000;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "51;";

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "203;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 10;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1629;
				taid = 1;
				type = 0;
			}
		}
	}
}

