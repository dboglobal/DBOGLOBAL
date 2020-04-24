CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1090;
	title = 109002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 109009;
				ctype = 1;
				idx = 1655301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 109008;
				area = 109001;
				goal = 109004;
				m0fz = "45.000000";
				m0widx = 4;
				scitem = 99069;
				sort = 109005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-105.000000";
				m0ttip = 109015;
				rwd = 100;
				taid = 1;
				title = 109002;
				gtype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 10901;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 109015;
				taid = 3;
				esctype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655301;";
			}
			CDboTSCheckClrQst
			{
				and = "1085;";
				flink = 1;
				flinknextqid = "1086;";
				not = 0;
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
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 109008;
			gtype = 2;
			oklnk = 2;
			area = 109001;
			goal = 109004;
			sort = 109005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 109002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 109014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 109001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1655301;";
			}
		}
	}
}

