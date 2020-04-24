CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2422;
	title = 242202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 242207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3183112;";
			}
			CDboTSCheckClrQst
			{
				and = "2419;";
				flink = 1;
				flinknextqid = "2423;";
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
				conv = 242209;
				ctype = 1;
				idx = 3183112;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 0;
				idx0 = 4611102;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7114123;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 242208;
				m3widx = 14;
				m1fy = "0.000000";
				sort = 242205;
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "683.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 242201;
				goal = 242204;
				m0fz = "861.000000";
				m0widx = 14;
				m1ttip = 242216;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1216.000000";
				m3ttip = 242216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "861.000000";
				m2fx = "763.000000";
				m2ttip = 242215;
				m0fx = "487.000000";
				m0ttip = 242215;
				m3pat = 2;
				title = 242202;
				m1fx = "487.000000";
				m2fz = "929.000000";
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
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 242214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 242201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183112;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 242208;
			gtype = 2;
			oklnk = 2;
			area = 242201;
			goal = 242204;
			sort = 242205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 242202;
		}
	}
}

