CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2408;
	title = 240802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 545;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 240822;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 4;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2408;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;5;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98019;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98019;
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 240807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "3148104;";
			}
			CDboTSCheckClrQst
			{
				and = "2407;";
				flink = 1;
				flinknextqid = "2409;2410;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 240809;
				ctype = 1;
				idx = 3148104;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2408;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98019;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 240808;
				m1fx = "1161.400024";
				m1fy = "-34.936001";
				sort = 240805;
				m0fy = "-20.091999";
				m0pat = 0;
				m1fz = "419.079987";
				grade = 132203;
				m0fx = "913.710022";
				m0ttip = 240815;
				m1pat = 0;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				title = 240802;
				gtype = 2;
				area = 240801;
				goal = 240804;
				m0fz = "616.729980";
				m0widx = 14;
				m1ttip = 240816;
				scitem = 98019;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 240814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 240801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3148104;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 240808;
			gtype = 2;
			oklnk = 2;
			area = 240801;
			goal = 240804;
			sort = 240805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 240802;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98019;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 240807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
	}
}

