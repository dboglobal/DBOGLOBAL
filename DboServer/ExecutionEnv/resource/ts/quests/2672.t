CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2672;
	title = 267202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 585;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 267222;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2672;
				start = 0;
				taid = 1;
				uof = 0;
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
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "4652101;";
			}
			CDboTSCheckClrQst
			{
				and = "2671;";
				flink = 1;
				flinknextqid = "2673;";
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
				conv = 267209;
				ctype = 1;
				idx = 4652101;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2672;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 267208;
				gtype = 3;
				area = 267201;
				goal = 267204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 267205;
				stype = 2;
				taid = 1;
				title = 267202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 267214;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 267201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5312107;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267208;
			gtype = 3;
			oklnk = 2;
			area = 267201;
			goal = 267204;
			sort = 267205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267202;
		}
	}
}

