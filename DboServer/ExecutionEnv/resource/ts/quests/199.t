CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 199;
	title = 19902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 19907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "7571105;";
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 5;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99170;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 19908;
			gtype = 1;
			oklnk = 2;
			area = 19901;
			goal = 19904;
			sort = 19905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 19902;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "5;4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSItemUpgrade
			{
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActOpenWindow
			{
				show = 1;
				taid = 3;
				tblidx = 20005;
				wtype = 5;
			}
			CDboTSActNPCConv
			{
				conv = 19909;
				ctype = 1;
				idx = 7571105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 19908;
				gtype = 1;
				area = 19901;
				goal = 19904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 19905;
				stype = 64;
				taid = 1;
				title = 19902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 19914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 19901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7571105;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99170;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 19907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

