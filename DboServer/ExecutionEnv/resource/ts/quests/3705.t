CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3705;
	title = 370502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 370507;
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
				npcidx = "1502102;";
			}
			CDboTSCheckClrQst
			{
				and = "3704;";
				flink = 0;
				flinknextqid = "3706;";
				not = 0;
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
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 370509;
				ctype = 1;
				idx = 1502102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 370508;
				gtype = 1;
				area = 370501;
				goal = 370504;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 370505;
				stype = 2;
				taid = 1;
				title = 370502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 370514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 370501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1501107;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 370508;
			gtype = 1;
			oklnk = 2;
			area = 370501;
			goal = 370504;
			sort = 370505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 370502;
		}
	}
}

