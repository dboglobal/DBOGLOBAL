CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6877;
	title = 687702;

	CNtlTSGroup
	{
		gid = 0;

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
				stype = 3;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 687707;
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
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "4513104;";
			}
			CDboTSCheckClrQst
			{
				and = "6874;";
				flink = 0;
				flinknextqid = "6880;";
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
				conv = 687709;
				ctype = 1;
				idx = 4513104;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 430001;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 687708;
				gtype = 1;
				area = 687701;
				goal = 687704;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 687705;
				stype = 64;
				taid = 1;
				title = 687702;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 687708;
			gtype = 1;
			oklnk = 2;
			area = 687701;
			goal = 687704;
			sort = 687705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 687702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 687714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 687701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4513104;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 687707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 430001;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

