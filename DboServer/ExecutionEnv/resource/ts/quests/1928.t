CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 1928;
	title = 192802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 192820;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSSkillUse
			{
				idx = 320101;
				rp = 0;
				stype = 6;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 192807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 8;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4752202;";
			}
			CDboTSCheckClrQst
			{
				and = "380;";
				flink = 1;
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
				conv = 192809;
				ctype = 1;
				idx = 4752202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 192808;
				gtype = 1;
				area = 192801;
				goal = 192804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 192805;
				stype = 64;
				taid = 1;
				title = 192802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 192814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 192801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752202;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 192808;
			gtype = 1;
			oklnk = 2;
			area = 192801;
			goal = 192804;
			sort = 192805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 192802;
		}
	}
}

