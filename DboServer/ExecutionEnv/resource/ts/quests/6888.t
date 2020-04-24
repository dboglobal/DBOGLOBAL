CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6888;
	title = 688802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 688807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 2097024;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "4512105;";
			}
			CDboTSCheckClrQst
			{
				and = "6887;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckClrQst
			{
				and = "6891;6894;6897;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688809;
				ctype = 1;
				idx = 4512105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 688808;
				m1fx = "5451.779785";
				m2fz = "1379.910034";
				m1fy = "0.000000";
				sort = 688805;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 688801;
				goal = 688804;
				m0fz = "1381.479980";
				m0widx = 1;
				m1ttip = 688816;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "356.769989";
				m2fx = "5373.720215";
				m2ttip = 688817;
				m0fx = "6383.040039";
				m0ttip = 688815;
				title = 688802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 688801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688808;
			gtype = 1;
			oklnk = 2;
			area = 688801;
			goal = 688804;
			sort = 688805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688802;
		}
	}
}

