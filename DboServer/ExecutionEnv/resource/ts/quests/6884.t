CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6884;
	title = 688402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 688407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4151114;";
			}
			CDboTSCheckClrQst
			{
				and = "6881;";
				flink = 0;
				not = 0;
			}
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

			CDboTSSkillUse
			{
				idx = 20912;
				rp = 0;
				stype = 4;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688409;
				ctype = 1;
				idx = 4151114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 688408;
				gtype = 1;
				area = 688401;
				goal = 688404;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 688405;
				stype = 64;
				taid = 1;
				title = 688402;
			}
			CDboTSActItem
			{
				iidx0 = 430002;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 688401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151114;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688408;
			gtype = 1;
			oklnk = 2;
			area = 688401;
			goal = 688404;
			sort = 688405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688402;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 430002;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

