CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 380;
	title = 38002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSBindStone
			{
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActOpenWindow
			{
				show = 1;
				taid = 3;
				tblidx = 20003;
				wtype = 5;
			}
			CDboTSActNPCConv
			{
				conv = 38009;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 38008;
				area = 38001;
				goal = 38004;
				m0fz = "-2661.899902";
				m0widx = 1;
				scitem = -1;
				sort = 38005;
				stype = 64;
				m0fy = "-43.973999";
				m0pat = 2;
				grade = 132203;
				m0fx = "3227.989990";
				m0ttip = 38015;
				rwd = 100;
				taid = 1;
				title = 38002;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 38014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 38001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 38007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				and = "203;";
				flink = 1;
				flinknextqid = "1928;1929;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 38008;
			gtype = 1;
			oklnk = 2;
			area = 38001;
			goal = 38004;
			sort = 38005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 38002;
		}
	}
}

