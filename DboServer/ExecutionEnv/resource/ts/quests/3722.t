CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3722;
	title = 372202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 372207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "7071112;";
			}
			CDboTSCheckClrQst
			{
				and = "3721;";
				flink = 0;
				flinknextqid = "3723;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372209;
				ctype = 1;
				idx = 7071112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 372208;
				gtype = 1;
				area = 372201;
				goal = 372204;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 372205;
				stype = 2;
				taid = 1;
				title = 372202;
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
			cont = 372208;
			gtype = 1;
			oklnk = 2;
			area = 372201;
			goal = 372204;
			sort = 372205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 372201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7076104;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

