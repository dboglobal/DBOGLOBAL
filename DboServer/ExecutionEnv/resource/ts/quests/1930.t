CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 1930;
	title = 193002;

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
				tblidx = 193020;
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
				idx = 521171;
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
			stdiag = 193007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 32;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4142101;";
			}
			CDboTSCheckClrQst
			{
				and = "580;";
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
				conv = 193009;
				ctype = 1;
				idx = 4142101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 193008;
				gtype = 1;
				area = 193001;
				goal = 193004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 193005;
				stype = 64;
				taid = 1;
				title = 193002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 193014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 193001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4142101;";
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
			cont = 193008;
			gtype = 1;
			oklnk = 2;
			area = 193001;
			goal = 193004;
			sort = 193005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 193002;
		}
	}
}

